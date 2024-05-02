/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 17:38:04 by akernot           #+#    #+#             */
/*   Updated: 2024/05/02 16:20:16 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "errno.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

/**
 * @brief checks if any philosopher has died in a previous cycle or this
 * philosopher has just died to prevent the program from announcing any
 * more actions and stop the program.
 * @param data the program's data structure. Used to check if any other
 * philosopher has died.
 * @param state the state of the current philosopher. Used to check if
 * this philosopher has just died and updates the data variable.
 * @return true if any philosopher has died, otherwise false.
 * @remark the mutex is unlocked in this function because of the 25 line limit
 * in the calling function.
*/
static t_bool	check_dead(t_program_data *data, t_state state)
{
	if (data->philo_died == true)
	{
		pthread_mutex_unlock(&data->announcing);
		return (true);
	}
	if (state == dead)
		data->philo_died = true;
	return (false);
}

/**
 * @remark state could be queried through a case...switch statement however
 * the style guide forbids them so an if statement is used instead.
*/
void	announce(t_program_data *data, int philo_id, t_state state)
{
	t_timeval	current_time;
	int64_t		current_timestamp;

	pthread_mutex_lock(&(data->announcing));
	if (check_dead(data, state))
		return ;
	gettimeofday(&current_time, NULL);
	current_timestamp = timestamp(&(data->time_0), &(current_time));
	printf("%li %i ", current_timestamp, philo_id);
	if (state == sleeping)
		printf("is sleeping\n");
	else if (state == thinking)
		printf("is thinking\n");
	else if (state == eating)
		printf("is eating\n");
	else if (state == fork_taken)
	{
		printf("has taken a fork\n");
		printf("%li %i has taken a fork\n", current_timestamp, philo_id);
	}
	else if (state == dead)
		printf("died\n");
	pthread_mutex_unlock(&(data->announcing));
}

pthread_t	*create_philos(t_program_data *data, t_fork *forks)
{
	t_packet	*packet;
	pthread_t	*philos;
	int			i;

	i = 0;
	philos = malloc(sizeof(pthread_t) * data->num_philos);
	packet = malloc(sizeof(t_packet) * data->num_philos);
	while (i < data->num_philos)
	{
		packet[i].data = data;
		packet[i].left_fork = &forks[i];
		packet[i].right_fork = &forks[(i + 1) % data->num_philos];
		packet[i].philo_id = i + 1;
		if (pthread_create(&philos[i], 0, init_philo, &packet[i]))
			return (0);
		++i;
	}
	usleep(40);
	free(packet);
	return (philos);
}

static void	start(t_program_data *data)
{
	t_bool			result;
	t_fork			*forks;
	pthread_t		*philos;

	forks = create_forks(data);
	if (forks == NULL)
		return ;
	gettimeofday(&(data->time_0), NULL);
	pthread_mutex_init(&(data->announcing), NULL);
	philos = create_philos(data, forks);
	if (philos == NULL)
		return ;
	while (1)
	{
		pthread_mutex_lock(&data->announcing);
		result = data->philo_died || data->philos_fin == data->num_philos;
		pthread_mutex_unlock(&data->announcing);
		if (result == true)
			break ;
	}
	wait_out(data, philos);
	free(forks);
	free(philos);
}

int	main(int ac, char *av[])
{
	t_program_data	data;

	if (ac != 5 && ac != 6)
		return (0);
	data.num_times_philo_eat = -1;
	data.num_philos = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	data.philo_died = false;
	data.philos_fin = 0;
	if (ac == 6)
		data.num_times_philo_eat = ft_atoi(av[5]);
	start(&data);
	return (0);
}
